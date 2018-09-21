type event;
type ipcCallback('a) = (. event, 'a) => unit;

[@bs.send] [@bs.scope "ipcRenderer"]
external on: (BsElectron.Window.electronT, string, ipcCallback('a)) => unit =
  "";

[@bs.send] [@bs.scope "ipcRenderer"]
external removeListener:
  (BsElectron.Window.electronT, string, ipcCallback('a)) => unit =
  "";

[@bs.send] [@bs.scope "ipcRenderer"]
external send: (BsElectron.Window.electronT, string, Js.t('a)) => unit =
  "send";
