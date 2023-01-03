#pragma once

class Timestep
{
public:
	Timestep(float timestep = 0.0f) : m_Timestep(timestep) {}

	operator float() { return m_Timestep; }

	float GetSeconds() { return m_Timestep; }

	float GetMilliSeconds() { return m_Timestep * 1000; }

private:
	float m_Timestep; //unit: second
};