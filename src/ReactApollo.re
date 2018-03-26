open ApolloClient;

open ApolloTypes;

[@bs.module] external gql : ApolloTypes.gql = "graphql-tag";

type results = {loading: bool};

module ApolloProvider = {
  [@bs.module "react-apollo"]
  external reactClass : ReasonReact.reactClass = "ApolloProvider";
  let make = (~client: generatedApolloClient, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props={"client": client},
      children,
    );
};

type apolloOptions = {
  .
  "query": gql,
  "variables": Js.Json.t,
  /* "updateQuery": Js.t, TODO: ? */
};

type apolloData = {
  .
  "loading": Js.boolean,
  "data": Js.Json.t,
  "error": apolloError,
  "refetch": Js.Json.t => Js.Promise.t(apolloData),
  "networkStatus": int,
  "variables": Js.Json.t,
  "fetchMore": apolloOptions => Js.Promise.t(unit),
};

module Query = {
  [@bs.module "react-apollo"]
  external reactClass : ReasonReact.reactClass = "Query";
  let make = (~query, children: apolloData => ReasonReact.reactElement) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props={"query": gql(. query##query), "variables": query##variables},
      children,
    );
};