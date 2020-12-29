float2 complex_multiply(float2 a, float2 b) {
    return (float2)(a.s0 * b.s0 - a.s1 * b.s1A, a.s0 * b.s1 + a.s1 * b.s0);
}

float2 fft_step(uint n, uint m, uint i, global const float2* harmonics, bool invert) {
    const uint k = i / m;
    const uint j = i - k * m;

    const float w_sign = invert ? -1 : 1;

    const uint jj = (j > m / 2) ? (j - m / 2) : j;

    const float w_phase = w_sign * 2 * jj * M_PI_F / m;
    const float2 w = (float2)(cos(w_phase), sin(w_phase));

    const float2 harmonic_even = harmonics[k + jj];
    const float2 harmonic_odd = harmonics[k + jj + m / 2];

    if (j > m / 2) {
        return harmonic_even + complex_multiply(w, harmonic_odd);
    } else {
        return harmonic_even - complex_multiply(w, harmonic_odd);
    }
}

kernel void fft_step_kernel(uint n, global const float2* harmonics_in, global float2* harmonics_out,
                            uint m, bool invert) {
    for (uint i = get_global_id(0); i < n; i += get_global_size(0)) {
        harmonics_out[i] = fft_step(n, m, i, harmonics_in, invert);
    }
}

kernel void fft_shuffle_kernel(uint n, global const float2* harmonics_in,
                               global float2* harmonics_out) {
    for (uint i = get_global_id(0); i < n; i += get_global_size(0)) {
        uint reverse_index = 0;
        for (uint j = 1; j < n; j <<= 1) {
            reverse_index <<= 1;
            reverse_index += bool(i & j);
        }
        harmonics_out[reverse_index] = harmonics_in[i];
    }
}

kernel void fft_divide_step_kernel(uint n, global const float2* harmonics_in,
                               global float2* harmonics_out) {
    for (uint i = get_global_id(0); i < n; i += get_global_size(0)) {
        uint reverse_index = 0;
        harmonics_out[i] = harmonics_in[i] / n;
    }
}
