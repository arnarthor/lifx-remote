open BsElectron;

module BrowserWindow = BrowserWindow.MakeBrowserWindow(Messages);

module IpcMain = IpcMain.MakeIpcMain(Messages);

let mainWindow = ref(Js.null);
let tray = ref(Js.null);

/* TODO: Add runtime config */
let dev = true;

let createWindow = () => {
  mainWindow :=
    Js.Null.return(
      BrowserWindow.makeWindowConfig(
        ~width=if (dev) {1400} else {800},
        ~height=if (dev) {800} else {525},
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
    dev ? "http://localhost:1234" : {j|file://$bundleLocation|j},
  );

  if (dev) {
    BrowserWindow.openDevTools(Js.Null.getExn(mainWindow^));
  };

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

App.on(
  `Ready,
  () => {
    IpcMain.on((. _event, message) =>
      switch (message) {
      | `TurnOnAllLights => Lifx.turnOnAll()
      | `TurnOffAllLights => Lifx.turnOffAll()
      | `SetLightStatuses(lightStatuses) => Js.log("All lights")
      | `RefreshLightsList => Js.log("Discover")
      }
    );
    createTray();
  },
);
App.Dock.hide();
