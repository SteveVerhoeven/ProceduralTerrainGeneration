#pragma once

class GameObject;
class Command
{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;

	protected:

};