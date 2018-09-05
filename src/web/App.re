let component = ReasonReact.statelessComponent("App");

module IpcRenderer = BsElectron.IpcRenderer.MakeIpcRenderer(Messages);

IpcRenderer.on((. _event, message) =>
  switch (message) {
  | `LightStatus(statuses) => Js.log(statuses)
  }
);

let make = (~message, _children) => {
  ...component,
  render: _self =>
    <div>
      <header> <h1> {ReasonReact.string(message)} </h1> </header>
      <button onClick={_ => IpcRenderer.send(`TurnOnAllLights)}>
        {ReasonReact.string("Turn on all lights")}
      </button>
      <button onClick={_ => IpcRenderer.send(`TurnOffAllLights)}>
        {ReasonReact.string("Turn off all lights")}
      </button>
      <button onClick={_ => IpcRenderer.send(`SetLightStatuses([]))}>
        {ReasonReact.string("Turn off lights")}
      </button>
      <p>
        {
          ReasonReact.string(
            "To get started, edit src/App.re and save to reload.",
          )
        }
      </p>
    </div>,
};
