[@bs.val] external hot: bool = "module.hot";

[@bs.val] external accept: unit => unit = "module.hot.accept";
%raw
"require('glamor/reset');";

ReactDOMRe.renderToElementWithId(<TestApp />, "index");

if (hot) {
  accept();
};
