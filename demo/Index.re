ReactDOMRe.renderToElementWithId(
  <Script url="https://www.gstatic.com/charts/loader.js">
    ...{remote =>
      switch (remote) {
      | Script.NotAsked => React.string("not asked")
      | Script.Loading => React.string("loading")
      | Script.Success => React.string("success")
      | Script.Failure => React.string("failure")
      }
    }
  </Script>,
  "index1",
);
ReactDOMRe.renderToElementWithId(
  <Script url="https://www.gstatic.com/charts/loader.js">
    ...{remote =>
      switch (remote) {
      | Script.NotAsked => React.string("not asked")
      | Script.Loading => React.string("loading asked")
      | Script.Success => React.string("success")
      | Script.Failure => React.string("failure")
      }
    }
  </Script>,
  "index2",
);
ReactDOMRe.renderToElementWithId(
  <Script url="https://www.gstatic.asdfasdfcom/charts/loader.js">
    ...{remote =>
      switch (remote) {
      | Script.NotAsked => React.string("not asked")
      | Script.Loading => React.string("loading asked")
      | Script.Success => React.string("success")
      | Script.Failure => React.string("failure")
      }
    }
  </Script>,
  "index3",
);
ReactDOMRe.renderToElementWithId(
  <Script url="https://www.gstatic.asdfasdfcom/charts/loader.js">
    ...{remote =>
      switch (remote) {
      | Script.NotAsked => React.string("not asked")
      | Script.Loading => React.string("loading asked")
      | Script.Success => React.string("success")
      | Script.Failure => React.string("failure")
      }
    }
  </Script>,
  "index4",
);
ReactDOMRe.renderToElementWithId(
  <Script url="https://www.gstatic.asdfasdfcom/charts/loader.js">
    ...{remote =>
      switch (remote) {
      | Script.NotAsked => React.string("not asked")
      | Script.Loading => React.string("loading asked")
      | Script.Success => React.string("success")
      | Script.Failure => React.string("failure")
      }
    }
  </Script>,
  "index3",
);
