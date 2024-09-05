import { useState } from "react";
import ThreeDScene from "./components/ThreeDScene";
import PythonRunner from "./components/PythonRunner";
import Loading from "./components/Loading";
import OutputDisplay from "./components/OutputDisplay";

const App = () => {
  const [output, setOutput] = useState<string>("(loading...)");
  const [isLoading, setIsLoading] = useState<boolean>(true);

  return (
    <div className="App">
      <header className="App-header">
        {isLoading ? <Loading /> : <OutputDisplay output={output} />}
        <ThreeDScene />
        <PythonRunner setOutput={setOutput} setIsLoading={setIsLoading} />
      </header>
    </div>
  );
};

export default App;
