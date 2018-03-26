open ReactApollo;

ReactDOMRe.renderToElementWithId(
  <ApolloProvider client=Client.client> <Page /> </ApolloProvider>,
  "index",
);