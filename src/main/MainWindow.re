open BsElectron;

module TestAppBrowserWindow =
  BrowserWindow.MakeBrowserWindow(TestAppMessages);
module BrowserWindow = BrowserWindow.MakeBrowserWindow(Messages);

module IpcMain = IpcMain.MakeIpcMain(Messages);

let mainWindow = ref(Js.null);
let testAppWindow = ref(Js.null);
let tray = ref(Js.null);

/* TODO: Add runtime config */
let dev = true;

let createTestApp = () => {
  testAppWindow :=
    Js.Null.return(
      TestAppBrowserWindow.makeWindowConfig(~width=559, ~height=2000, ())
      ->TestAppBrowserWindow.make,
    );

  let bundleLocation =
    Node_path.join([|
      Belt.Option.getExn([%bs.node __dirname]),
      "./build/index.html",
    |]);

  TestAppBrowserWindow.loadURL(
    Js.Null.getExn(testAppWindow^),
    /* TODO: Setup env checks */
    dev ? "http://localhost:1235" : {j|file://$bundleLocation|j},
  );
  let {x, y}: ElectronPositioner.coordinates =
    Js.Null.getExn(testAppWindow^)
    ->ElectronPositioner.make
    ->ElectronPositioner.calculate(`TopLeft);

  TestAppBrowserWindow.setPosition(
    Js.Null.getExn(testAppWindow^),
    ~x,
    ~y,
    ~animate=false,
  );
  if (dev) {
    TestAppBrowserWindow.openDevTools(Js.Null.getExn(testAppWindow^));
  };
};

let createWindow = () => {
  mainWindow :=
    Js.Null.return(
      BrowserWindow.makeWindowConfig(
        ~width=800,
        ~height=if (dev) {1600} else {525},
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
      | `SetLightStatuses(_) as signal =>
        TestAppBrowserWindow.send(Js.Null.getExn(testAppWindow^), signal)
      | `RefreshLightsList => Js.log("Discover")
      }
    );
    createTray();
    createTestApp();
  },
);
App.Dock.hide();
