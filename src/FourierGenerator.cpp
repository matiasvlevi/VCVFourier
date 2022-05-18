#include "utils.hpp"

struct FourierGenerator : Module
{
  bool dirtyTextFrequencies = false;
  bool dirtyTextAmplitudes = false;

  float phase = 0.f;
  float sum = 0.f;

  trigFunc trig = &std::sin;

  std::string frequenciesText;
  std::string amplitudesText;

  std::vector<float> frequencies;
  std::vector<float> amplitudes;

  enum ParamId
  {
    WAVE_TYPE,
    MASTER_AMPLITUDE,
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
    configParam(MASTER_AMPLITUDE, 1.f, 4.f, 2.f, "Amplitude", " V");
    configParam(WAVE_TYPE, 0.f, 1.f, 0.f, "Wave delay");
    configOutput(FOURIER_OUTPUT, "Output wave");
  }

  void fromJson(json_t *rootJ) override
  {
    Module::fromJson(rootJ);

    json_t *frequenciesTextJ = json_object_get(rootJ, "freq");
    json_t *amplitudesTextJ = json_object_get(rootJ, "amp");

    if (frequenciesTextJ)
      frequenciesText = json_string_value(frequenciesTextJ);

    if (amplitudesTextJ)
      amplitudesText = json_string_value(amplitudesTextJ);

    dirtyTextFrequencies = true;
    dirtyTextAmplitudes = true;
  }

  json_t *dataToJson() override
  {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "freq", json_stringn(frequenciesText.c_str(), frequenciesText.size()));
    json_object_set_new(rootJ, "amp", json_stringn(amplitudesText.c_str(), amplitudesText.size()));
    return rootJ;
  }

  void dataFromJson(json_t *rootJ) override
  {
    json_t *frequenciesTextJ = json_object_get(rootJ, "freq");
    json_t *amplitudesTextJ = json_object_get(rootJ, "amp");

    if (frequenciesTextJ)
      frequenciesText = json_string_value(frequenciesTextJ);

    if (amplitudesTextJ)
      amplitudesText = json_string_value(amplitudesTextJ);

    dirtyTextFrequencies = true;
    dirtyTextAmplitudes = true;
  }

  void onReset() override
  {
    amplitudesText = "";
    frequenciesText = "";

    amplitudes = {0.f};
    frequencies = {0.f};

    sum = 0.f;
    phase = 0.f;

    dirtyTextFrequencies = true;
    dirtyTextAmplitudes = true;
  }

  void process(const ProcessArgs &args) override
  {
    phase += 10.f * args.sampleTime; // 100Hz stock frequency
    if (phase >= 0.5f)
      phase -= 1.f;

    if (params[WAVE_TYPE].getValue() > 0.5f)
      trig = &std::cos;
    else
      trig = &std::sin;

    sum = Utils::Waves::fourier(phase, sum, frequencies, amplitudes, trig);

    outputs[FOURIER_OUTPUT]
        .setVoltage(params[MASTER_AMPLITUDE].getValue() * sum);
  }
};

struct FormulaTextField : LedDisplayTextField
{
  FourierGenerator *module;
  char modifiableValues;

  void step() override
  {

    LedDisplayTextField::step();
    if (module)
    {
      switch (modifiableValues)
      {
      case 'F':
        if (module->dirtyTextFrequencies)
        {
          setText(module->frequenciesText);
          module->frequencies = Utils::split(module->frequenciesText, ',');
          module->dirtyTextFrequencies = false;
        }
        break;
      case 'A':
        if (module->dirtyTextAmplitudes)
        {
          setText(module->amplitudesText);
          module->amplitudes = Utils::split(module->amplitudesText, ',');
          module->dirtyTextAmplitudes = false;
        }
        break;
      }
    }
  }

  void onChange(const ChangeEvent &e) override
  {
    if (module)
    {
      switch (modifiableValues)
      {
      case 'A':
        module->amplitudesText = getText();
        module->amplitudes = Utils::split(module->amplitudesText, ',');
        break;
      case 'F':
        module->frequenciesText = getText();
        module->frequencies = Utils::split(module->frequenciesText, ',');
        break;
      }
    }
  }
};

struct FormulaDisplay : LedDisplay
{
  void setModule(FourierGenerator *module, char modifiableValues)
  {
    FormulaTextField *textField = createWidget<FormulaTextField>(Vec(0, 0));
    textField->box.size = box.size;
    textField->multiline = false;
    textField->module = module;
    textField->modifiableValues = modifiableValues;
    textField->bgColor = nvgRGBA(0x00, 0x00, 0x00, 0x00);
    textField->color = nvgRGB(0x00, 0xff, 0x01);

    addChild(textField);
  }
};

struct FourierGeneratorWidget : ModuleWidget
{
  FourierGeneratorWidget(FourierGenerator *module)
  {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/FourierGenerator.svg")));

    // Frequencies textbox (S_i)
    FormulaDisplay *frequenciesTextbox = createWidget<FormulaDisplay>(mm2px(Vec(18, 12)));
    frequenciesTextbox->box.size = mm2px(Vec(78, 10));
    frequenciesTextbox->setModule(module, 'F'); // This textbox instance will modify Frequencies
    addChild(frequenciesTextbox);

    // Amplitudes textbox (L_i)
    FormulaDisplay *amplitudesTextbox = createWidget<FormulaDisplay>(mm2px(Vec(18, 26)));
    amplitudesTextbox->box.size = mm2px(Vec(78, 10));
    amplitudesTextbox->setModule(module, 'A'); // This textbox instance will modify Amplitudes
    addChild(amplitudesTextbox);

    addParam(createParam<CKSS>(mm2px(Vec(20, 100)), module, FourierGenerator::WAVE_TYPE));
    addParam(createParam<RoundBlackKnob>(mm2px(Vec(43, 100)), module, FourierGenerator::MASTER_AMPLITUDE));

    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(110, 90)), module, FourierGenerator::FOURIER_OUTPUT));
  }
};

Model *modelFourierGenerator = createModel<FourierGenerator, FourierGeneratorWidget>("FourierGenerator");