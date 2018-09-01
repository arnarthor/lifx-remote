open BsElectron;

let win = ref(Js.null);

let createWindow = () => {
  win :=
    Js.Null.return(
      BrowserWindow.make({
        width: 1600,
        height: 1000,
        frame:
          switch (Os.platform()) {
          | Some(`MacOS) => false
          | _ => true
          },
        titleBarStyle:
          switch (Os.platform()) {
          | Some(`MacOS) => "hidden"
          | _ => "default"
          },
      }),
    );

  let bundleLocation =
    Node_path.join([|
      Belt.Option.getExn([%bs.node __dirname]),
      "./build/index.html",
    |]);
  BrowserWindow.loadURL(
    Js.Null.getExn(win^),
    /* TODO: Setup env checks */
    true ? "http://localhost:1234" : {j|file://$bundleLocation|j},
  );
  if (true) {
    BrowserWindow.openDevTools(Js.Null.getExn(win^));
  };
  BrowserWindow.on(Js.Null.getExn(win^), `Closed, () => win := Js.null);
};

App.on(
  `Ready,
  () => {
    createWindow();
    ();
  },
);
