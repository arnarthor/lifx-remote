open Css;
open! Styles;
%raw
"require('./lightbulb.css')";

type state = {allLightsOn: bool};

type action =
  | AllLightsOn(bool);

let component = ReasonReact.reducerComponent(__MODULE__);

module IpcRenderer = BsElectron.IpcRenderer.MakeIpcRenderer(Messages);

IpcRenderer.on((. _event, message) =>
  switch (message) {
  | `LightStatus(statuses) => Js.log(statuses)
  }
);

let root =
  style([
    minHeight(`vh(100.)),
    backgroundColor(black),
    fontSize(px(16)),
    color(hex("ff")),
    padding(px(20)),
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
    width(px(150)),
    height(px(150)),
    borderRadius(pct(50.)),
    boxShadows(shadow),
    transition(~duration=1000, "all"),
    selector(
      ">label",
      [
        txtShadow,
        color(hex("ffffff")),
        marginLeft(`auto),
        marginRight(`auto),
        marginBottom(px(2)),
        transition(~duration=1000, "all"),
      ],
    ),
  ]);
};

let make = _children => {
  ...component,
  initialState: () => {allLightsOn: false},
  reducer: (action, _state) =>
    switch (action) {
    | AllLightsOn(on) =>
      ReasonReact.UpdateWithSideEffects(
        {allLightsOn: on},
        (
          self =>
            if (self.state.allLightsOn) {
              IpcRenderer.send(`TurnOnAllLights);
            } else {
              IpcRenderer.send(`TurnOffAllLights);
            }
        ),
      )
    },
  render: self =>
    <div className=root>
      <div className={lightContainer(self.state.allLightsOn)}>
        <label> {ReasonReact.string("All lights")} </label>
        <input
          className="l"
          type_="checkbox"
          checked={self.state.allLightsOn}
          onChange={
            e => self.send(AllLightsOn(ReactEvent.Form.target(e)##checked))
          }
        />
      </div>
    </div>,
};