open Css;
open! Types;
open! Styles;

let lights = [
  {id: 0, name: "Bedroom", supportsColor: true, color: `Red, turnedOn: true},
  {
    id: 1,
    name: "Kitchen",
    supportsColor: false,
    color: `White,
    turnedOn: true,
  },
  {
    id: 2,
    name: "Entrance",
    supportsColor: false,
    color: `White,
    turnedOn: false,
  },
  {
    id: 3,
    name: "TV Room",
    supportsColor: true,
    color: `Yellow,
    turnedOn: true,
  },
  {
    id: 4,
    name: "Bathroom",
    supportsColor: false,
    color: `White,
    turnedOn: false,
  },
  {
    id: 5,
    name: "Office",
    supportsColor: false,
    color: `White,
    turnedOn: false,
  },
];

type state = list(light);

type action =
  | ToggleLight(int, bool)
  | ChangeColor(int, color);

let component = ReasonReact.reducerComponent(__MODULE__);

module IpcRenderer = BsElectron.IpcRenderer.MakeIpcRenderer(TestAppMessages);

let root =
  style([
    display(`grid),
    gridGap(px(2)),
    gridTemplateColumns([pct(50.), pct(50.)]),
    minHeight(`vh(100.)),
    minWidth(`vw(100.)),
    backgroundColor(white),
  ]);

let lightTile = (turnedOn, color) =>
  style([
    display(`flex),
    height(pct(100.)),
    alignItems(`center),
    justifyContent(`center),
    Css.color(
      switch (turnedOn, color) {
      | (false, _) => Css.white
      | (_, `White)
      | (_, `Yellow) => Css.black
      | (_, `Blue)
      | (_, `Red) => Css.white
      },
    ),
    backgroundColor(
      switch (turnedOn, color) {
      | (false, _) => Css.black
      | (_, `Yellow) => Css.yellow
      | (_, `Red) => Css.red
      | (_, `White) => Css.white
      | (_, `Blue) => Css.blue
      },
    ),
  ]);

let make = _children => {
  ...component,
  initialState: () => lights,
  reducer: (action, state) =>
    switch (action) {
    | ToggleLight(id, turnedOn) =>
      let (beforeLight, rest) =
        Belt.List.splitAt(state, id)->Belt.Option.getExn;
      let [lightAtId, ...rest] = rest;
      let newLightState = {...lightAtId, turnedOn};
      ReasonReact.Update(
        Belt.List.concat(beforeLight, [newLightState, ...rest]),
      );
    | _ => ReasonReact.NoUpdate
    },
  didMount: ({send, state}) =>
    IpcRenderer.on((. _event, message) =>
      switch (message) {
      | `SetLightStatuses(statuses) =>
        Belt.List.forEach(statuses, ((lightId, turnedOn)) =>
          send(ToggleLight(lightId, turnedOn))
        )
      | `RefreshLightsList => IpcRenderer.send(`LightStatus(state))
      | _ => Js.log("Hello")
      }
    ),
  render: ({state, send}) =>
    <div className=root>
      {
        Belt.List.map(state, light =>
          <div
            className={lightTile(light.turnedOn, light.color)}
            onClick={_ => send(ToggleLight(light.id, !light.turnedOn))}>
            {ReasonReact.string(light.name)}
          </div>
        )
        |> Belt.List.toArray
        |> ReasonReact.array
      }
    </div>,
};
