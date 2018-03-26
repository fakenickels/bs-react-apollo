open ApolloClient;

open ApolloTypes;

[@bs.module] external gql : ApolloTypes.gql = "graphql-tag";

let unwrap_bool = (b: option(bool)) =>
  switch (b) {
  | Some(value) => Js.Nullable.return(Js.Boolean.to_js_boolean(value))
  | None => Js.Nullable.undefined
  };

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
  "data": Js.Nullable.t(Js.Json.t),
  "error": Js.Nullable.t(apolloError),
  "refetch": Js.Nullable.t(Js.Json.t) => Js.Promise.t(apolloData),
  "networkStatus": int,
  "variables": Js.Nullable.t(Js.Json.t),
  "fetchMore": apolloOptions => Js.Promise.t(unit),
};

module Query = {
  [@bs.module "react-apollo"]
  external reactClass : ReasonReact.reactClass = "Query";
  let make =
      (
        ~query,
        ~fetchPolicy: option(string)=?,
        ~ssr: option(bool)=?,
        ~notifyOnNetworkStatusChange: option(bool)=?,
        ~pollInterval: option(int)=?,
        children: apolloData => ReasonReact.reactElement,
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
      children,
    );
};

type mutationOptions = {
  .
  "variables": Js.Nullable.t(Js.Json.t),
  "refetchQueries": Js.Nullable.t(array(string)),
};

type apolloMutation =
  Js.Nullable.t(mutationOptions) => Js.Promise.t(apolloData);

module Mutation = {
  [@bs.module "react-apollo"]
  external reactClass : ReasonReact.reactClass = "Mutation";
  let make =
      (
        ~mutation,
        ~onError: option(unit => unit)=?,
        ~onCompleted: option(unit => unit)=?,
        children: (apolloMutation, apolloData) => ReasonReact.reactElement,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props=
        Js.Nullable.(
          {
            "mutation": gql(. mutation##query),
            "variables": mutation##variables,
            "onError": fromOption(onError),
            "onCompleted": fromOption(onCompleted),
          }
        ),
      children,
    );
};

module ApolloConsumer = {
  [@bs.module "react-apollo"]
  external reactClass : ReasonReact.reactClass = "ApolloConsumer";
  let make = (children: generatedApolloClient => ReasonReact.reactElement) =>
    ReasonReact.wrapJsForReason(~reactClass, ~props=Js.Obj.empty(), children);
};