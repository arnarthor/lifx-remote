open Css;
open! Styles;

%raw
"require('./lightbulb.css')";

type state = {lights: list(Types.light)};

type action =
  | Discover(list(Types.light))
  | ToggleLight(int, bool);

let component = ReasonReact.reducerComponent(__MODULE__);

module IpcRenderer = ElectronIpcRenderer;

let root =
  style([
    minHeight(px(525)),
    maxHeight(px(525)),
    backgroundColor(black),
    fontSize(px(16)),
    color(hex("ff")),
    padding(px(20)),
    overflowY(`scroll),
  ]);

let lightsContainer =
  style([
    height(pct(100.)),
    width(pct(100.)),
    display(`grid),
    gridTemplateColumns([pct(33.), pct(33.), pct(33.)]),
    marginTop(px(-50)),
  ]);

let lightContainer = (bulbOn: bool) => {
  let outBoxShadow = boxShadow(~blur=px(17), ~spread=px(9));
  let insetBoxShadow = boxShadow(~blur=px(15), ~spread=px(5), ~inset=true);
  let off = [
    rgba(0, 0, 0, 0.5)->outBoxShadow,
    rgba(255, 255, 255, 0.2)->insetBoxShadow,
  ];
  let on = [
    rgba(232, 212, 60, 0.75)->outBoxShadow,
    rgba(232, 212, 60, 0.8)->insetBoxShadow,
  ];
  let shadow = bulbOn ? on : off;

  let txtShadow = textShadow(~blur=px(3));
  let on = rgba(232, 212, 60, 0.5)->txtShadow;
  let off = rgba(0, 0, 0, 0.8)->txtShadow;
  let txtShadow = bulbOn ? off : on;

  style([
    display(`flex),
    padding(em(1.)),
    flexDirection(`column),
    justifyContent(`center),
    marginLeft(`auto),
    marginRight(`auto),
    marginTop(px(50)),
    width(px(150)),
    height(px(150)),
    borderRadius(pct(50.)),
    boxShadows(shadow),
    transition(~duration=1000, "all"),
    width(pct(50.)),
    selector(
      ">label",
      [
        txtShadow,
        color(hex("ffffff")),
        userSelect(`none),
        marginLeft(`auto),
        marginRight(`auto),
        marginBottom(px(2)),
        transition(~duration=1000, "all"),
      ],
    ),
  ]);
};

let calculateLights = (state, id, turnedOn) => {
  let (beforeLight, rest) =
    Belt.List.splitAt(state.lights, id)->Belt.Option.getExn;
  let [lightAtId, ...rest] = rest;
  let newLightState = {...lightAtId, turnedOn};
  Belt.List.concat(beforeLight, [newLightState, ...rest]);
};

let make = _children => {
  ...component,
  initialState: () => {lights: []},
  reducer: (action, state) =>
    switch (action) {
    | ToggleLight(id, turnedOn) =>
      ReasonReact.UpdateWithSideEffects(
        {lights: calculateLights(state, id, turnedOn)},
        (_ => IpcRenderer.send(`SetLightStatuses([(id, turnedOn)]))),
      )
    | Discover(lights) => ReasonReact.Update({lights: lights})
    },
  didMount: ({send}) => {
    IpcRenderer.on((_event, message) =>
      switch (message) {
      | `LightStatus(lights) => send(Discover(lights))
      }
    );
    IpcRenderer.send(`RefreshLightsList);
  },
  render: ({state, send}) =>
    <div className=root>
      <div className=lightsContainer>
        {
          Belt.List.map(state.lights, light =>
            <div
              key=light.id->string_of_int
              className={lightContainer(light.turnedOn)}>
              <label> {ReasonReact.string(light.name)} </label>
              <input
                className="l"
                type_="checkbox"
                checked={light.turnedOn}
                onChange={_ => send(ToggleLight(light.id, !light.turnedOn))}
              />
            </div>
          )
          |> Belt.List.toArray
          |> ReasonReact.array
        }
      </div>
    </div>,
};
