/* Simple resampler based on bsnes's ruby audio library, but with the same
 * interface as Blargg's Game_Music_Emu Fir_Resampler */

#ifndef __RESAMPLER_H
#define __RESAMPLER_H

#undef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#undef CLAMP
#undef short_clamp
#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define short_clamp(n) ((short) CLAMP((n), -32768, 32767))

class Resampler
{
    private:
        short *internal_buffer;
        int   buffer_max;
        int   filled;

        double r_step;
        double r_frac;
        int    r_left[4], r_right[4];

        double
        hermite (double mu1, double a, double b, double c, double d)
        {
            const double tension = 0.0; //-1 = low, 0 = normal, 1 = high
            const double bias    = 0.0; //-1 = left, 0 = even, 1 = right

            double mu2, mu3, m0, m1, a0, a1, a2, a3;

            mu2 = mu1 * mu1;
            mu3 = mu2 * mu1;

            m0  = (b - a) * (1 + bias) * (1 - tension) / 2;
            m0 += (c - b) * (1 - bias) * (1 - tension) / 2;
            m1  = (c - b) * (1 + bias) * (1 - tension) / 2;
            m1 += (d - c) * (1 - bias) * (1 - tension) / 2;

            a0 = +2 * mu3 - 3 * mu2 + 1;
            a1 =      mu3 - 2 * mu2 + mu1;
            a2 =      mu3 -     mu2;
            a3 = -2 * mu3 + 3 * mu2;

            return (a0 * b) + (a1 * m0) + (a2 * m1) + (a3 * c);
        }

    public:
        Resampler (int num_samples)
        {
            internal_buffer = new short[num_samples];
            buffer_max = num_samples;
            r_frac = 0.0;
            filled = 0;
        }

        ~Resampler ()
        {
            delete [] internal_buffer;
        }

        void
        time_ratio (double ratio)
        {
            this->r_step = ratio;
            clear ();
        }

        void
        clear (void)
        {
            filled = 0;
            r_frac = 0;
            r_left [0] = r_left [1] = r_left [2] = r_left [3] = 0;
            r_right[0] = r_right[1] = r_right[2] = r_right[3] = 0;
        }

        short *
        buffer (void)
        {
            return internal_buffer + filled;
        }

        void
        write (int size)
        {
            filled += MIN (max_write (), size);
        }

        void
        read (short *data, int size)
        {
            int i_position = 0;
            int o_position = 0;

            while (o_position < size && i_position < filled)
            {
                int s_left = internal_buffer[i_position];
                int s_right = internal_buffer[i_position + 1];

                if (r_step == 1.0)
                {
                    data[o_position] = (short) s_left;
                    data[o_position + 1] = (short) s_right;

                    o_position += 2;

                    continue;
                }

                r_left [0] = r_left [1];
                r_left [1] = r_left [2];
                r_left [2] = r_left [3];
                r_left [3] = s_left;

                r_right[0] = r_right[1];
                r_right[1] = r_right[2];
                r_right[2] = r_right[3];
                r_right[3] = s_right;

                while (r_frac <= 1.0 && o_position < size)
                {
                    data[o_position]     = short_clamp (hermite (r_frac, r_left [0], r_left [1], r_left [2], r_left [3]));
                    data[o_position + 1] = short_clamp (hermite (r_frac, r_right[0], r_right[1], r_right[2], r_right[3]));

                    o_position += 2;

                    r_frac += r_step;
                }

                if (r_frac > 1.0)
                {
                    r_frac -= 1.0;
                    i_position += 2;
                }
            }

            memmove (internal_buffer,
                     internal_buffer + i_position,
                     sizeof (short) * (filled - i_position));
            filled -= i_position;
        }

        int
        max_write (void)
        {
            return buffer_max - filled;
        }

        void
        buffer_size (int size)
        {
            delete[] internal_buffer;
            internal_buffer = new short[size];
            buffer_max = size;
            clear ();
        }

        int
        avail (void)
        {
            int size = 0;
            double temp_pos = r_frac;

            while ((temp_pos) < (filled >> 1))
            {
                size += 2;
                temp_pos += r_step;
            }

            return size;
        }
};

#endif /* __RESAMPLER_H */
