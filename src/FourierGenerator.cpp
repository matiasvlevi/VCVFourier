#include "utils.hpp"

#define TWO_PI 2.f * M_PI

typedef float (*trigFunc)(float);

struct FourierGenerator : Module
{
  bool dirty = false;
  float phase = 0.f;
  float sum = 0.f;
  trigFunc trig = &std::sin;
  std::string content;
  std::vector<float> data;
  enum ParamId
  {
    WAVE_TYPE,
    AMPLITUDE,
    PARAMS_LEN
  };
  enum InputId
  {
    INPUTS_LEN
  };
  enum OutputId
  {
    FOURIER_OUTPUT,
    OUTPUTS_LEN
  };
  enum LightId
  {
    LIGHTS_LEN
  };

  FourierGenerator()
  {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configParam(AMPLITUDE, 1.f, 4.f, 1.f, "Amplitude", " V");
    configParam(WAVE_TYPE, 0.f, 1.f, 1.f, "Wave type", " SIN/COS");

    configOutput(FOURIER_OUTPUT, "Output wave");
  }

  void onReset() override
  {
    dirty = true;
  }

  void process(const ProcessArgs &args) override
  {

    phase += args.sampleTime;

    if (phase >= 0.5f)
      phase -= 1.f;

    if (params[WAVE_TYPE].getValue() > 0.5f)
      trig = &std::sin;
    else
      trig = &std::cos;

    sum = 0.f;
    for (int n = 0; n < (int)data.size(); n++)
    {
      sum += trig(TWO_PI * phase * data[n]) / (n + 1);
    }

    outputs[FOURIER_OUTPUT].setVoltage(params[AMPLITUDE].getValue() * sum);
  }
};

struct FormulaTextField : LedDisplayTextField
{
  FourierGenerator *module;

  void step() override
  {
    LedDisplayTextField::step();
    if (module && module->dirty)
    {
      setText(module->content);

      module->dirty = false;
    }
  }

  void onChange(const ChangeEvent &e) override
  {
    if (module)
    {
      module->content = getText();
      module->data = Utils::split(module->content, ',');
    }
  }
};

struct FormulaDisplay : LedDisplay
{
  void setModule(FourierGenerator *module)
  {
    FormulaTextField *textField = createWidget<FormulaTextField>(Vec(0, 0));
    textField->box.size = box.size;
    textField->multiline = true;
    textField->module = module;
    textField->color = nvgRGB(0x00, 0xff, 0x01);
    textField->bgColor = nvgRGBA(0x00, 0xff, 0x01, 0x00);
    addChild(textField);
  }
};

struct FourierGeneratorWidget : ModuleWidget
{
  FourierGeneratorWidget(FourierGenerator *module)
  {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/FourierGenerator.svg")));

    // TextBox
    FormulaDisplay *formulaDisplay = createWidget<FormulaDisplay>(mm2px(Vec(3.5f, 12)));
    formulaDisplay->box.size = mm2px(Vec(78, 70));
    formulaDisplay->setModule(module);
    addChild(formulaDisplay);

    // Switches
    addParam(createParam<CKSS>(mm2px(Vec(20, 90)), module, FourierGenerator::WAVE_TYPE));

    // Knobs
    addParam(createParam<RoundBlackKnob>(mm2px(Vec(30, 90)), module, FourierGenerator::AMPLITUDE));

    // Output
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7, 90)), module, FourierGenerator::FOURIER_OUTPUT));
  }
};

Model *modelFourierGenerator = createModel<FourierGenerator, FourierGeneratorWidget>("FourierGenerator");