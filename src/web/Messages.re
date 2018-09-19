type mainToRendererMessages = [ | `LightStatus(list((string, bool)))];

type rendererToMainMessages = [
  | `TurnOnAllLights
  | `TurnOffAllLights
  | `SetLightStatuses(list((int, bool)))
  | `RefreshLightsList
];
