open ReactApollo;

let component = ReasonReact.statelessComponent("Page");

let int_from_opt = (~default: int=0, option: option(int)) =>
  switch (option) {
  | None => default
  | Some(a) => a
  };

module CountQuery = [%graphql
  {|
    query films {
      allFilms {
        totalCount
      }
    }
|}
];

let make = _children => {
  ...component,
  render: _self => {
    let countQuery = CountQuery.make();
    let parse = countQuery##parse;
    <Query query=countQuery>
      ...(
           result =>
             <div>
               <h1> ("Star Wars 2: " |> ReasonReact.stringToElement) </h1>
               (
                 result##loading |> Js.to_bool ?
                   "Loading" |> ReasonReact.stringToElement :
                   (
                     switch (result##data |> Js.Nullable.toOption) {
                     | None => ReasonReact.nullElement
                     | Some(data) =>
                       let result = parse(data);
                       switch (result##allFilms) {
                       | None => "No Fils" |> ReasonReact.stringToElement
                       | Some(films) =>
                         "Films Found: "
                         ++ (
                           films##totalCount |> int_from_opt |> string_of_int
                         )
                         |> ReasonReact.stringToElement
                       };
                     }
                   )
               )
             </div>
         )
    </Query>;
  },
};