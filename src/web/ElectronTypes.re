[@bs.deriving jsConverter]
type mainToRenderer = [ | `LightStatus];

[@bs.deriving jsConverter]
type rendererToMain = [ | `SetLightStatus | `RefreshLightsList];
