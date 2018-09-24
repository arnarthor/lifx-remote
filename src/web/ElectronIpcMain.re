type event;
type ipcCallback = (event, string) => unit;
type messageCallback('a) = (event, 'a) => unit;

[@bs.module "electron"] [@bs.scope "ipcMain"]
external on: (string, ipcCallback) => unit = "";
let on = (cb: messageCallback(ElectronTypes.rendererToMain)) =>
  on("message", (event, arg) =>
    cb(event, arg->Js.Json.parseExn->BsElectron.Json.fromValidJson)
  );

[@bs.module "electron"] [@bs.scope "ipcMain"]
external removeListener:
  (string, messageCallback(ElectronTypes.rendererToMain)) => unit =
  "";
let removeListener = removeListener("message");
