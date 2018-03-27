open ApolloTypes;

[@bs.module] external gql : ApolloTypes.gql = "graphql-tag";

let unwrap_bool = (b: option(bool)) =>
  switch (b) {
  | Some(value) => Js.Nullable.return(Js.Boolean.to_js_boolean(value))
  | None => Js.Nullable.undefined
  };

module type Config = {let query: string; type t; let parse: Js.Json.t => t;};

module Create = (Config: Config) => {
  let queryGql = gql(. Config.query);
  type apolloOptions = {
    .
    "query": gql,
    "variables": Js.Json.t,
  };
  type apolloData = {
    .
    "loading": Js.boolean,
    "called": Js.boolean,
    "data": Js.Nullable.t(Js.Json.t),
    "error": Js.Nullable.t(apolloError),
    "refetch":
      [@bs.meth] (Js.Nullable.t(Js.Json.t) => Js.Promise.t(apolloData)),
    "networkStatus": int,
    "variables": Js.Nullable.t(Js.Json.t),
    "fetchMore": [@bs.meth] (apolloOptions => Js.Promise.t(unit)),
  };
  type data =
    | Loading
    | Error(apolloError)
    | Data(Config.t)
    | NoData;
  type apollo = {
    data,
    refetch: option(Js.Json.t) => data,
    fetchMore: (~variables: Js.Json.t) => Js.Promise.t(unit),
    networkStatus: int,
  };
  let apolloDataToReason: apolloData => data =
    apolloData =>
      switch (
        Js.to_bool(apolloData##loading),
        Js.Null_undefined.toOption(apolloData##data),
        Js.Null_undefined.toOption(apolloData##error),
      ) {
      | (true, _, _) => Loading
      | (false, Some(data), None) => Data(Config.parse(data))
      | (false, _, Some(error)) => Error(error)
      | (false, None, None) => NoData
      };
  [@bs.module "react-apollo"]
  external reactClass : ReasonReact.reactClass = "Query";
  let convertJsInputToReason = apolloData => {
    data: apolloDataToReason(apolloData##data),
    refetch: variables =>
      variables |> Js.Null_undefined.fromOption |> apolloData##refetch,
    fetchMore: (~variables) =>
      {"variables": variables, "query": queryGql} |> apolloData##fetchMore,
    networkStatus: apolloData##networkStatus,
  };
  let make =
      (
        ~query,
        ~fetchPolicy: option(string)=?,
        ~ssr: option(bool)=?,
        ~notifyOnNetworkStatusChange: option(bool)=?,
        ~pollInterval: option(int)=?,
        children: apollo => ReasonReact.reactElement,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props=
        Js.Nullable.(
          {
            "query": gql(. query##query),
            "variables": query##variables,
            "fetchPolicy": fromOption(fetchPolicy),
            "ssr": unwrap_bool(ssr),
            "notifyOnNetworkStatusChange":
              unwrap_bool(notifyOnNetworkStatusChange),
            "pollInterval": fromOption(pollInterval),
          }
        ),
      apolloData =>
      apolloData |> convertJsInputToReason |> children
    );
};
