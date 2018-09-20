type color = [ | `Yellow | `Red | `White | `Blue];
type light = {
  id: int,
  name: string,
  supportsColor: bool,
  color,
  turnedOn: bool,
};
