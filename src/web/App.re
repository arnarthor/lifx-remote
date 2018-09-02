let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: _self =>
    <div>
      <header> <h1> {ReasonReact.string(message)} </h1> </header>
      <p>
        {
          ReasonReact.string(
            "To get started, edit src/App.re and save to reload.",
          )
        }
      </p>
    </div>,
};
