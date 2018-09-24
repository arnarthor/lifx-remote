type lightStatusUpdate = {
  id: int,
  turnedOn: bool,
};
type mainToRenderer = [ | `LightStatus(list(Types.light))];

type rendererToMain = [
  | `SetLightStatus(lightStatusUpdate)
  | `RefreshLightsList
];
