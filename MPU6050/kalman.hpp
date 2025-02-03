#ifndef KALMAN_HPP_
#define KALMAN_HPP_

namespace bbb
{

    template <typename Type = double>
    class kalman
    {
        Type Q_angle;   // Process noise variance for the accelerometer
        Type Q_bias;    // Process noise variance for the gyro bias
        Type R_measure; // Measurement noise variance

        Type kalman_angle{0}; // The angle calculated by the Kalman filter
        Type bias{0};         // The gyro bias calculated by the Kalman filter

        Type theta_gyr; // Unbiased gyroscope angle

        Type P_00{0}, P_01{0}, P_10{0}, P_11{0}; // Error covariance matrix 2x2 - (m × n) - P_mn

    public:
        kalman(Type a = 0.001, Type b = 0.003, Type m = 0.03) : Q_angle(a),
                                                                Q_bias(b),
                                                                R_measure(m) {}

        kalman(const kalman &) = delete;
        kalman &operator=(const kalman &) = delete;

        void init_angle(Type angle)
        {
            kalman_angle = angle;
        }

        Type angle(Type angle, Type gyro, Type dt = 1)
        {
            //--------------------------------------------------------------------------------------------

            /*  Time update (Predict)   */

            // Project the state ahead
            theta_gyr = gyro - bias;
            kalman_angle += dt * theta_gyr; // angle = angle + dt.gyr_rate

            // Project the error covariance ahead
            P_00 += dt * (dt * P_11 - P_01 - P_10 + Q_angle);
            P_01 -= dt * P_11;
            P_10 -= dt * P_11;
            P_11 += Q_bias * dt;

            //--------------------------------------------------------------------------------------------

            /*  Measurement Update (Correct)    */

            // Compute the Kalman gain
            Type K_1, K_2;             // Kalman gain - 2x1 (m × n) column vector
            Type S = P_00 + R_measure; // Estimate error (innovation covariance)
            K_1 = P_00 / S;
            K_2 = P_10 / S;

            // Update estimate with measurement (zk = angle - kalman_angle)
            kalman_angle += K_1 * (angle - kalman_angle);
            bias += K_2 * (angle - kalman_angle);

            // Update the error covariance
            Type P00_temp = P_00;
            Type P01_temp = P_01;

            P_00 -= K_1 * P00_temp;
            P_01 -= K_1 * P01_temp;
            P_10 -= K_2 * P00_temp;
            P_11 -= K_2 * P01_temp;

            return kalman_angle;
        }

        /*
        Sources for the Kalman Filter :

        => https://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it/

        => https://polen.itu.edu.tr:8443/server/api/core/bitstreams/1b757cf0-dfa2-4339-b21c-60a10825b8d8/content
        */
    };

}
#endif