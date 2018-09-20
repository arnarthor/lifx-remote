let message = __MODULE__;
type mainToRendererMessages = [ | `LightStatus(list(Types.light))];

type rendererToMainMessages = [
  | `TurnOnAllLights
  | `TurnOffAllLights
  | `SetLightStatuses(list((int, bool)))
  | `RefreshLightsList
];
