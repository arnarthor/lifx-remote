type event;
type ipcCallback('a) = (. event, 'a) => unit;

[@bs.send] [@bs.scope "ipcRenderer"]
external on: (BsElectron.Window.electronT, string, ipcCallback('a)) => unit =
  "";

let on = (t, message, cb) =>
  on(t, ElectronTypes.mainToRendererToJs(message), cb);

[@bs.send] [@bs.scope "ipcRenderer"]
external removeListener:
  (BsElectron.Window.electronT, string, ipcCallback('a)) => unit =
  "";

let removeListener = (t, message, cb) =>
  removeListener(t, ElectronTypes.mainToRendererToJs(message), cb);

[@bs.send] [@bs.scope "ipcRenderer"]
external send: (BsElectron.Window.electronT, string, 'a) => unit = "send";

let send = (t, message, data) =>
  send(t, ElectronTypes.rendererToMainToJs(message), data);
