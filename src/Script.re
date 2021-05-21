type element;

[@bs.val] external createElement: string => element = "document.createElement";

[@bs.set] external setSrc: (element, string) => unit = "src";

[@bs.set] external setOnload: (element, unit => unit) => unit = "onload";

[@bs.set] external setOnerror: (element, unit => unit) => unit = "onerror";

[@bs.val]
external appendChild: element => element = "document.body.appendChild";

type observer = {
  onload: unit => unit,
  onerror: unit => unit,
};

type remote =
  | NotAsked
  | Loading
  | Success
  | Failure;

type state = {
  remote,
  id: int,
};

type action =
  | Initialize
  | CreateScript
  | Onload
  | Onerror
  | FlushOnload
  | FlushOnerror;

let loadedScripts = ref(Belt.Set.String.empty);

let erroredScripts = ref(Belt.Set.String.empty);

let observers = ref(Belt.Map.String.empty);

let idCount = ref(0);

[@react.component]
let make = (~url, ~children, ()) => {
  ReactCompat.useRecordApi({
    ...ReactCompat.component,

    initialState: () => {
      idCount := idCount^ + 1;
      {remote: NotAsked, id: idCount^};
    },
    didMount: self => self.send(Initialize),
    willUnmount: self =>
      /* delete callbacks when component unmounts*/
      observers :=
        Belt.Map.String.set(
          observers^,
          url,
          Belt.Map.String.get(observers^, url)
          |> Belt.Option.getWithDefault(_, Belt.Map.Int.empty)
          |> Belt.Map.Int.remove(_, self.state.id),
        ),
    reducer: (action, state: state) =>
      switch (action) {
      | Initialize =>
        let loaded = Belt.Set.String.has(loadedScripts^, url);
        let errored = Belt.Set.String.has(erroredScripts^, url);
        let loading = Belt.Map.String.has(observers^, url);
        switch (loaded, errored, loading) {
        | (true, _, _) => Update({...state, remote: Success})
        | (_, true, _) => Update({...state, remote: Failure})
        | (_, _, true) =>
          UpdateWithSideEffects(
            {...state, remote: Loading},
            self =>
              observers :=
                Belt.Map.String.set(
                  observers^,
                  url,
                  Belt.Map.String.get(observers^, url)
                  |> Belt.Option.getWithDefault(_, Belt.Map.Int.empty)
                  |> Belt.Map.Int.set(
                       _,
                       self.state.id,
                       {
                         onerror: () => self.send(Onerror),
                         onload: () => self.send(Onload),
                       },
                     ),
                ),
          )
        | (false, false, false) =>
          UpdateWithSideEffects(
            {...state, remote: Loading},
            self => {
              observers :=
                Belt.Map.String.set(
                  observers^,
                  url,
                  Belt.Map.String.get(observers^, url)
                  |> Belt.Option.getWithDefault(_, Belt.Map.Int.empty)
                  |> Belt.Map.Int.set(
                       _,
                       self.state.id,
                       {
                         onerror: () => self.send(Onerror),
                         onload: () => self.send(Onload),
                       },
                     ),
                );
              self.send(CreateScript);
            },
          )
        };
      | CreateScript =>
        SideEffects(
          self => {
            let script = createElement("script");
            setSrc(script, url);
            setOnload(
              script,
              () => {
                loadedScripts := Belt.Set.String.add(loadedScripts^, url);
                self.send(FlushOnload);
              },
            );
            setOnerror(
              script,
              () => {
                erroredScripts := Belt.Set.String.add(erroredScripts^, url);
                self.send(FlushOnerror);
              },
            );
            appendChild(script) |> ignore;
          },
        )
      | Onload => Update({...state, remote: Success})
      | Onerror => Update({...state, remote: Failure})
      | FlushOnload =>
        SideEffects(
          _self =>
            Belt.Map.String.get(observers^, url)
            |> Belt.Option.map(_, observers' =>
                 Belt.Map.Int.forEach(observers', (_id, observer) =>
                   observer.onload()
                 )
               )
            |> ignore,
        )
      | FlushOnerror =>
        SideEffects(
          _self =>
            Belt.Map.String.get(observers^, url)
            |> Belt.Option.map(_, observers' =>
                 Belt.Map.Int.forEach(observers', (_id, observer) =>
                   observer.onerror()
                 )
               )
            |> ignore,
        )
      },
    render: self => children(self.state.remote),
  });
};
