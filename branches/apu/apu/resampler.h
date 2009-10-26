/* Simple resampler based on bsnes's ruby audio library, but with the same
 * interface as Blargg's Game_Music_Emu Fir_Resampler */

#ifndef __RESAMPLER_H
#define __RESAMPLER_H

#include "ring_buffer.h"

#undef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#undef CLAMP
#undef short_clamp
#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define short_clamp(n) ((short) CLAMP((n), -32768, 32767))

class Resampler : ring_buffer
{
    protected:

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
        Resampler (int num_samples) : ring_buffer (num_samples << 1)
        {
            r_frac = 0.0;
        }

        ~Resampler ()
        {
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
            ring_buffer::clear ();
            r_frac = 0;
            r_left [0] = r_left [1] = r_left [2] = r_left [3] = 0;
            r_right[0] = r_right[1] = r_right[2] = r_right[3] = 0;
        }

        void
        read (short *data, int size)
        {
            int i_position = ring_buffer::start >> 1;
            short *internal_buffer = (short *) ring_buffer::buffer;
            int o_position = 0;
            int consumed = 0;

            while (o_position < size && consumed < ring_buffer::buffer_size)
            {
                int s_left = internal_buffer[i_position];
                int s_right = internal_buffer[i_position + 1];

                if (r_step == 1.0)
                {
                    data[o_position] = (short) s_left;
                    data[o_position + 1] = (short) s_right;

                    o_position += 2;
                    i_position = (i_position + 2) % (ring_buffer::buffer_size >> 1);
                    consumed += 2;

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
                    i_position = (i_position + 2) % (ring_buffer::buffer_size >> 1);
                    consumed += 2;
                }
            }

            ring_buffer::size -= consumed << 1;
            ring_buffer::start = (ring_buffer::start + (consumed << 1)) % ring_buffer::buffer_size;
        }

        bool
        push (short *src, int size)
        {
            if (max_write () < size)
                return false;

            ring_buffer::push ((unsigned char *) src, size << 1);

            return true;
        }

        int
        max_write (void)
        {
            return ring_buffer::space_empty () >> 1;
        }

        void
        buffer_size (int size)
        {
            ring_buffer::resize (size << 1);
        }

        int
        avail (void)
        {
            return (int) floor (((size >> 2) - r_frac) / r_step) * 2;
        }
};

#endif /* __RESAMPLER_H */
