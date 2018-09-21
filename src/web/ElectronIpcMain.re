type event;
type ipcCallback('a) = (. event, Js.t('a)) => unit;
[@bs.module "electron"] [@bs.scope "ipcMain"]
external on: (string, ipcCallback('a)) => unit = "";
[@bs.module "electron"] [@bs.scope "ipcMain"]
external removeListener: (string, ipcCallback('a)) => unit = "";
