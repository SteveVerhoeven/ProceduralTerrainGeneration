#pragma once

class Generator
{
	public:
		Generator(const std::string& name);
		virtual ~Generator() = default;

		Generator(const Generator&) = delete;
		Generator(Generator&&) noexcept = delete;
		Generator& operator=(const Generator&) = delete;
		Generator& operator=(Generator&&) noexcept = delete;

	protected:

	private:
		std::string m_Name;
};