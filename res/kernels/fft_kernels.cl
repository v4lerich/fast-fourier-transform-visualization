float2 complex_multiply(float2 a, float2 b) {
    return (float2)(a.s0 * b.s0 - a.s1 * b.s1, a.s0 * b.s1 + a.s1 * b.s0);
}

float2 fft_step(uint m, uint i, global const float2* harmonics, bool invert) {
    const uint k = m * (i / m);
    const uint j = i % (m / 2);
    const uint jj = i % m;

    const float w_sign = invert ? -1 : 1;
    const float w_phase = w_sign * 2 * j * M_PI_F / m;
    const float2 w = (float2)(cos(w_phase), sin(w_phase));

    const float2 harmonic_even = harmonics[k + j];
    const float2 harmonic_odd = harmonics[k + j + m / 2];

    float butterfly_sign = (jj < m / 2) ? +1 : -1;
    return harmonic_even + butterfly_sign * complex_multiply(w, harmonic_odd);
}

kernel void fft_step_kernel(uint n, global const float2* harmonics_in, global float2* harmonics_out,
                            uint m, uint invert) {
    for (uint i = get_global_id(0); i < n; i += get_global_size(0)) {
        harmonics_out[i] = fft_step(m, i, harmonics_in, invert != 0);
    }
}

kernel void fft_shuffle_kernel(uint n, global float2* harmonics) {
    for (uint i = get_global_id(0); i < n; i += get_global_size(0)) {
        uint reverse_index = 0;
        for (uint j = 1; j < n; j <<= 1) {
            reverse_index <<= 1;
            if (i & j) reverse_index++;
        }
        if (reverse_index < i) {
            float2 temporary = harmonics[i];
            harmonics[i] = harmonics[reverse_index];
            harmonics[reverse_index] = temporary;
        }
    }
}

kernel void fft_divide_step_kernel(uint n, global float2* harmonics) {
    for (uint i = get_global_id(0); i < n; i += get_global_size(0)) {
        harmonics[i] /= n;
    }
}
