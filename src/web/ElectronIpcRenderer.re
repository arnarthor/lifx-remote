type event;
type ipcCallback('a) = (. event, 'a) => unit;

[@bs.send] [@bs.scope "ipcRenderer"]
external on: (BsElectron.Window.electronT, string, ipcCallback('a)) => unit =
  "";
let on = (message, cb) => on(BsElectron.Window.electron, message, cb);

[@bs.send] [@bs.scope "ipcRenderer"]
external removeListener:
  (BsElectron.Window.electronT, string, ipcCallback('a)) => unit =
  "";
let removeListener = (message, cb) =>
  removeListener(BsElectron.Window.electron, message, cb);

[@bs.send] [@bs.scope "ipcRenderer"]
external send: (BsElectron.Window.electronT, string, 'a) => unit = "send";
let send = (message, data) =>
  send(BsElectron.Window.electron, message, data);
