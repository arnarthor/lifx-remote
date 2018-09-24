type mainToRenderer = [ | `LightStatus(list(Types.light))];

type rendererToMain = [
  | `SetLightStatuses(list((int, bool)))
  | `RefreshLightsList
];
