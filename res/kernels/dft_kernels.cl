float2 dft_harmonic(uint j, uint n, global const float* signal) {
    float2 harmonic = 0;

    for (uint i = 0; i < n; i++) {
        float phase = 2 * M_PI_F * i * j / n;
        harmonic += signal[i] * (float2)(cos(phase), sin(phase));
    }
    return harmonic / n;
}

kernel void dft_kernel(uint n, global const float* signal, uint m, global float2* harmonics) {
    for (uint j = get_global_id(0); j < m; j += get_global_size(0)) {
        harmonics[j] = dft_harmonic(j, n, signal);
    }
}

float inverse_dft_value(uint i, uint n, uint m, global const float2* harmonics) {
    float value = 0;

    for (uint j = 0; j < m; j++) {
        float phase = 2 * M_PI_F * i * j / n - atan2(harmonics[j].s1, harmonics[j].s0);
        value += hypot(harmonics[j].s0, harmonics[j].s1) * cos(phase);
    }
    return value;
}

kernel void inverse_dft_kernel(uint m, global const float2* harmonics, uint n, global float* signal) {
    for (uint i = get_global_id(0); i < n; i += get_global_size(0)) {
        signal[i] = inverse_dft_value(i, n, m, harmonics);
    }
}
