type event;
type ipcCallback('a) = (. event, 'a) => unit;

[@bs.send] [@bs.scope "ipcRenderer"]
external on: (BsElectron.Window.electronT, string, ipcCallback('a)) => unit =
  "";
let on = (message: ElectronTypes.mainToRenderer, cb) =>
  on(
    BsElectron.Window.electron,
    ElectronTypes.mainToRendererToJs(message),
    cb,
  );

[@bs.send] [@bs.scope "ipcRenderer"]
external send: (BsElectron.Window.electronT, string, 'a) => unit = "send";
let send = (message: ElectronTypes.rendererToMain, data) =>
  send(
    BsElectron.Window.electron,
    ElectronTypes.rendererToMainToJs(message),
    data,
  );

[@bs.send] [@bs.scope "ipcRenderer"]
external removeListener:
  (BsElectron.Window.electronT, string, ipcCallback('a)) => unit =
  "";

let removeListener = (message: ElectronTypes.mainToRenderer, cb) =>
  removeListener(
    BsElectron.Window.electron,
    ElectronTypes.mainToRendererToJs(message),
    cb,
  );
