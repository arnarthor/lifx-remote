open BsElectron;

let mainWindow = ref(Js.null);
let tray = ref(Js.null);

let createWindow = () => {
  mainWindow :=
    Js.Null.return(
      BrowserWindow.makeWindowConfig(
        ~width=800,
        ~height=525,
        ~frame=false,
        ~fullscreenable=false,
        ~resizeable=false,
        (),
      )
      ->BrowserWindow.make,
    );

  let bundleLocation =
    Node_path.join([|
      Belt.Option.getExn([%bs.node __dirname]),
      "./build/index.html",
    |]);

  BrowserWindow.loadURL(
    Js.Null.getExn(mainWindow^),
    /* TODO: Setup env checks */
    true ? "http://localhost:1234" : {j|file://$bundleLocation|j},
  );

  BrowserWindow.on(Js.Null.getExn(mainWindow^), `Blur, _ =>
    BrowserWindow.hide(Js.Null.getExn(mainWindow^))
  );
};

let showWindow = trayBounds => {
  let {x, y}: ElectronPositioner.coordinates =
    Js.Null.getExn(mainWindow^)
    ->ElectronPositioner.make
    ->ElectronPositioner.calculate(`TrayCenter(trayBounds));

  BrowserWindow.setPosition(
    Js.Null.getExn(mainWindow^),
    ~x,
    ~y,
    ~animate=false,
  );
  BrowserWindow.show(Js.Null.getExn(mainWindow^));
};

let toggleWindow = trayBounds => {
  if (mainWindow^ == Js.null) {
    createWindow();
  };
  switch (BrowserWindow.getVisibility(Js.Null.getExn(mainWindow^))) {
  | `Showing => BrowserWindow.hide(Js.Null.getExn(mainWindow^))
  | `Hidden => showWindow(trayBounds)
  };
};

let createTray = () => {
  tray :=
    Js.Null.return(
      Tray.make(
        Node.Path.join2(
          Belt.Option.getExn([%bs.node __dirname]),
          switch (Os.platform()) {
          | Some(`MacOS) => "../public/icon.png"
          | _ => "../public/favicon.ico"
          },
        ),
      ),
    );

  Tray.on(Js.Null.getExn(tray^), `Click, (_event, trayBounds) =>
    toggleWindow(trayBounds)
  );
};

App.on(`Ready, () => createTray());
App.Dock.hide();
