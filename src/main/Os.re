[@bs.deriving jsConverter]
type platforms = [
  | [@bs.as "aix"] `Aix
  | [@bs.as "darwin"] `MacOS
  | [@bs.as "freebsd"] `FreeBsd
  | [@bs.as "linux"] `Linux
  | [@bs.as "openbsd"] `OpenBsd
  | [@bs.as "sunos"] `SunOS
  | [@bs.as "win32"] `Windows
];

[@bs.module "os"] external platform: unit => string = "";

let platform = () => platform()->platformsFromJs;
