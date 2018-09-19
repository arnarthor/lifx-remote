open Css;

let component = ReasonReact.statelessComponent("App");

module IpcRenderer = BsElectron.IpcRenderer.MakeIpcRenderer(Messages);

IpcRenderer.on((. _event, message) =>
  switch (message) {
  | `LightStatus(statuses) => Js.log(statuses)
  }
);

let root = style([minHeight(`vh(100.)), backgroundColor(hex("000000"))]);

let make = _children => {
  ...component,
  render: _self =>
    <div className=root>
      <button onClick={_ => IpcRenderer.send(`TurnOnAllLights)}>
        {ReasonReact.string("Turn on all lights")}
      </button>
      <button onClick={_ => IpcRenderer.send(`TurnOffAllLights)}>
        {ReasonReact.string("Turn off all lights")}
      </button>
      <button onClick={_ => IpcRenderer.send(`SetLightStatuses([]))}>
        {ReasonReact.string("Turn off lights")}
      </button>
    </div>,
};
