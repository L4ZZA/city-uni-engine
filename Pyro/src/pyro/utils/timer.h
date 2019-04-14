#pragma once

namespace pyro
{
	class timer
	{
	public:
        /// \brief Starts the timer
        void Start();
        /// \brief returns the seconds passed since 
        /// the last Elapsed() call or since the Start().
        double Elapsed();

    private:
        double m_delta_time{ 0.0 };
        double m_last_frame{ 0.0 };
        bool m_started = false;
	};
}
