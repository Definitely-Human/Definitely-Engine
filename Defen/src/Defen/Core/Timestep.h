#pragma once

namespace Defen
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetSeconsds() const { return m_Time; }
		float GetMiliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}
