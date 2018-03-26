open ApolloInMemoryCache;

type dataObject = {
  .
  "__typename": string,
  "id": string,
};

let inMemoryCache =
  createInMemoryCache(~dataIdFromObject=(obj: dataObject) => obj##id, ());

let httpLink = ApolloLinks.createHttpLink(~uri="http://swapi.apis.guru/", ());

let createApolloClient =
    (
      ~cache,
      ~link,
      ~ssrMode=?,
      ~ssrForceFetchDelay=?,
      ~connectToDevTools=?,
      ~queryDeduplication=?,
      (),
    ) => {
  let apolloClientOptions = {
    "link": link,
    "cache": cache,
    "ssrMode": Js.Nullable.fromOption(ssrMode),
    "ssrForceFetchDelay": Js.Nullable.fromOption(ssrForceFetchDelay),
    "connectToDevTools": Js.Nullable.fromOption(connectToDevTools),
    "queryDeduplication": Js.Nullable.fromOption(queryDeduplication),
  };
  ApolloClient.createApolloClient(apolloClientOptions);
};

let client = createApolloClient(~cache=inMemoryCache, ~link=httpLink, ());