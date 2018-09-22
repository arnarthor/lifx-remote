type event;
type ipcCallback('a) = (. event, Js.t('a)) => unit;

[@bs.module "electron"] [@bs.scope "ipcMain"]
external on: (string, ipcCallback('a)) => unit = "";
let on = (message, cb) => on(ElectronTypes.rendererToMainToJs(message), cb);

[@bs.module "electron"] [@bs.scope "ipcMain"]
external removeListener: (string, ipcCallback('a)) => unit = "";
let removeListener = (message, cb) =>
  removeListener(ElectronTypes.rendererToMainToJs(message), cb);
