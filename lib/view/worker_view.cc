#include "worker_view.h"

#include <imgui.h>

#include <map>
#include <numbers>
#include <random>
#include <utility>

#include "view.h"

namespace fft_visualizer::view {

static std::vector<unsigned int> kLogNOptions = {6,  7,  8,  9,  10, 11, 12, 13,
                                                 14, 15, 16, 17, 18, 19, 20};
static std::map<SignalType, std::string> kSignalTypeOptions = {
    {SignalType::kHarmonic, "Harmonic"},
    {SignalType::kPolyharmonic, "Polyharmonic"},
};

template <typename T>
using Range = std::pair<T, T>;

static Range<float> kAmplitudeRange = {-100.0f, +100.0f};
static Range<float> kPhaseRange = {0, 2 * std::numbers::pi_v<float>};
static Range<float> kHarmonicRange = {0, +100};
static Range<int> kHarmonicsCountRange = {1, 100};

WorkerView::WorkerView(Model& model, std::string window_name)
    : model_{model}, window_name_{std::move(window_name)}, n_{GetNValue(0)} {}

void WorkerView::Render() {
    if (ImGui::Begin(window_name_.c_str())) {
        CenterText("Parameters");
        ImGui::Separator();

        RenderNOptionsCombo();
        ImGui::NewLine();

        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.35f);
        RenderSignalTypeCombo();
        ImGui::SameLine(0, 100.0f);
        if (ImGui::Button("Randomize")) {
            RandomizeSignalParameters();
        }

        ImGui::PopItemWidth();
        ImGui::Spacing();
        if (signal_type_ == SignalType::kHarmonic) {
            RenderHarmonicSignalParameters();
        } else if (signal_type_ == SignalType::kPolyharmonic) {
            RenderPolyharmonicSignalParameters();
        }
        ImGui::NewLine();
        ImGui::Dummy({0, 0});

        CenterText("Control");
        ImGui::Separator();
        if (ImGui::Button("Generate signal")) {
            GenerateSignal();
        }

        ImGui::End();
    }
}

auto WorkerView::GetWindowName() -> const std::string& { return window_name_; }

void WorkerView::RenderNOptionsCombo() {
    std::string preview = std::to_string(n_);
    if (ImGui::BeginCombo("N", preview.c_str())) {
        for (unsigned int i = 0; i < kLogNOptions.size(); i++) {
            const auto& value = GetNValue(i);
            bool is_selected = n_ == value;

            if (ImGui::Selectable(std::to_string(value).c_str(), &is_selected)) n_ = value;
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void WorkerView::RenderSignalTypeCombo() {
    std::string preview = kSignalTypeOptions[signal_type_];
    if (ImGui::BeginCombo("Signal type", preview.c_str())) {
        for (const auto& [signal_type, signal_type_name_] : kSignalTypeOptions) {
            bool is_selected = signal_type_ == signal_type;

            if (ImGui::Selectable(signal_type_name_.c_str(), &is_selected))
                signal_type_ = signal_type;
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void WorkerView::RenderHarmonicSignalParameters() {
    ImGui::DragFloat("Amplitude", &harmonic_parameters_.amplitude, 0.1f, kAmplitudeRange.first,
                     kAmplitudeRange.second);
    ImGui::DragFloat("Phase", &harmonic_parameters_.phase, 0.01f, kPhaseRange.second,
                     kPhaseRange.second);
    ImGui::DragFloat("Harmonic", &harmonic_parameters_.harmonic, 0.1f, kHarmonicRange.first,
                     kHarmonicRange.second);
}

void WorkerView::RenderPolyharmonicSignalParameters() {
    int harmonics_count = polyharmonic_parameters_.size();
    ImGui::DragInt("Harmonics count", &harmonics_count, 1, kHarmonicsCountRange.first,
                   kHarmonicsCountRange.second);
    polyharmonic_parameters_.resize(harmonics_count);

    ImGui::NewLine();
    if (ImGui::BeginChild("Harmonics", {0, 300.0f}, true,
                          ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
        for (size_t i = 0; i < polyharmonic_parameters_.size(); i++) {
            if (i != 0) {
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
            }

            ImGui::Text("Harmonic #%2d", i + 1);

            ImGui::PushID(i);
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.35f);

            ImGui::DragFloat("Amplitude", &polyharmonic_parameters_[i].amplitude, 0.1f,
                             kAmplitudeRange.first, kAmplitudeRange.second);
            ImGui::SameLine();
            ImGui::DragFloat("Phase", &polyharmonic_parameters_[i].phase, 0.01f, kPhaseRange.first,
                             kPhaseRange.second);

            ImGui::PopItemWidth();
            ImGui::PopID();
        }
        ImGui::EndChild();
    }
}

auto WorkerView::GetNValue(unsigned int index) -> unsigned int { return 1 << kLogNOptions[index]; }

void WorkerView::RandomizeSignalParameters() {
    std::mt19937 generator(random_device_());
    std::uniform_real_distribution<float> harmonic_distribution(kHarmonicRange.first,
                                                                kHarmonicRange.second);
    std::normal_distribution<float> amplitude_distribution((kAmplitudeRange.first + kAmplitudeRange.second) / 2,
                                                           (kAmplitudeRange.second - kAmplitudeRange.first) / 6);
    std::uniform_real_distribution<float> phase_distribution(kPhaseRange.first, kPhaseRange.second);

    harmonic_parameters_.amplitude = amplitude_distribution(generator);
    harmonic_parameters_.phase = phase_distribution(generator);
    harmonic_parameters_.harmonic = harmonic_distribution(generator);

    for (auto& harmonic : polyharmonic_parameters_) {
        harmonic.amplitude = amplitude_distribution(generator);
        harmonic.phase = phase_distribution(generator);
    }
}

void WorkerView::GenerateSignal() {
    std::unique_ptr<model::signal_generator::SignalGenerator> signal_generator;
    if (signal_type_ == SignalType::kHarmonic) {
        signal_generator = std::make_unique<model::signal_generator::HarmonicSignalGenerator>(
            harmonic_parameters_);
    } else if (signal_type_ == SignalType::kPolyharmonic) {
        signal_generator = std::make_unique<model::signal_generator::PolyharmonicSignalGenerator>(
            std::begin(polyharmonic_parameters_), std::end(polyharmonic_parameters_));
    }

    if (signal_generator) {
        const auto signal = signal_generator->Generate(n_);
        model_.SetInitialSignal(signal);
    }
}

}  // namespace fft_visualizer::view
