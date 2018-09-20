type mainToRendererMessages = [ | `LightStatus(list(Types.light))];

type rendererToMainMessages = [
  | `TurnOnAllLights
  | `TurnOffAllLights
  | `SetLightStatuses(list((int, bool)))
  | `RefreshLightsList
];
