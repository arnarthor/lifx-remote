[@bs.val] external hot: bool = "module.hot";

[@bs.val] external accept: unit => unit = "module.hot.accept";
%raw
"require('glamor/reset');";

ReactDOMRe.renderToElementWithId(<App />, "index");

if (hot) {
  accept();
};
