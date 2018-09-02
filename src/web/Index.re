[@bs.val] external hot: bool = "module.hot";

[@bs.val] external accept: unit => unit = "module.hot.accept";

ReactDOMRe.renderToElementWithId(
  <App message="Welcome to Parcel Reason React" />,
  "index",
);

if (hot) {
  accept();
};
