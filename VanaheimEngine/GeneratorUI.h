#pragma once
#include "UI.h"

#include <vector>

enum class GeneratorType
{
	NOISE,
	PROCEDURAL_GENERATION
};
struct GeneratorVariable
{
	GeneratorVariable(const GeneratorType& genType,
					  const UIButtonType& type,
					  const std::string& name, 
					  const std::vector<ObserverEvent>& events,
					  const DirectX::XMFLOAT2 range)
	{
		generatorType = genType;
		buttonType = type;
		varName = name;
		varEvents = events;
		varRange = range;
	}
	GeneratorVariable(const GeneratorType& genType,
					  const UIButtonType& type,
					  const std::string& name,
					  const std::vector<ObserverEvent>& events)
	{
		generatorType = genType;
		buttonType = type;
		varName = name;
		varEvents = events;
	}

	GeneratorType generatorType{};
	UIButtonType buttonType{};
	std::string varName{};
	std::vector<ObserverEvent> varEvents{};
	DirectX::XMFLOAT2 varRange{};
};
class GeneratorUI final : public UI
{
	public:
		GeneratorUI(const std::string& windowName, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size);
		~GeneratorUI() = default;

		GeneratorUI(const GeneratorUI&) = delete;
		GeneratorUI& operator=(const GeneratorUI&) = delete;
		GeneratorUI(GeneratorUI&&) = delete;
		GeneratorUI& operator=(GeneratorUI&&) = delete;
		
		void StoreVariable(const GeneratorVariable& genVar) { m_Variables.push_back(genVar); }
		
		virtual void ShowWindow() override;

	protected:
	private:		
		std::vector<GeneratorVariable> m_Variables;

		void DisplayVariables();

		// Types of buttons
		void UseArrowButton(const std::string& name, const std::vector<ObserverEvent>& events, const GeneratorType& genType);
		void UseListBox(const std::string& name, int type, const std::vector<ObserverEvent>& events);
		void UseSliderFloat(const std::string& name, const GeneratorType& genType, const DirectX::XMFLOAT2& range, const std::vector<ObserverEvent>& events);
		void UseSliderInt(const std::string& name, const GeneratorType& genType, const DirectX::XMFLOAT2& range, const std::vector<ObserverEvent>& events);

		// Reformatters
		bool DisplayArrows(const std::string& name, const std::vector<ObserverEvent>& events);

		void DisplayNoiseGenVariableValue(const std::string& name);
		void DisplayProcGenVariableValue(const std::string& name);
};