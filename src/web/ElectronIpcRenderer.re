open BsElectron;
type event;
type ipcCallback = (event, string) => unit;
type messageCallback('a) = (event, 'a) => unit;

[@bs.send] [@bs.scope "ipcRenderer"]
external on: (BsElectron.Window.electronT, string, ipcCallback) => unit = "";
let on = (cb: messageCallback(ElectronTypes.mainToRenderer)) =>
  on(Window.electron, "message", (event, arg) =>
    cb(event, arg->Js.Json.parseExn->Json.fromValidJson)
  );

[@bs.send] [@bs.scope "ipcRenderer"]
external send: (BsElectron.Window.electronT, string, 'a) => unit = "send";
let send = (arg: ElectronTypes.rendererToMain) =>
  send(Window.electron, "message", arg->Json.toValidJson->Js.Json.stringify);

[@bs.send] [@bs.scope "ipcRenderer"]
external removeListener:
  (BsElectron.Window.electronT, string, ipcCallback) => unit =
  "";
