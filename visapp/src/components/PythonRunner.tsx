import { useEffect } from "react";
import script from "../python/test.py";

interface PythonRunnerProps {
  setOutput: (output: string) => void;
  setIsLoading: (loading: boolean) => void;
}

const PythonRunner: React.FC<PythonRunnerProps> = ({
  setOutput,
  setIsLoading,
}) => {
  useEffect(() => {
    window.languagePluginLoader.then(() => {
      fetch(script)
        .then((src) => src.text())
        .then((code) => {
          window.pyodide.loadPackage([]).then(() => {
            const output = window.pyodide.runPython(code);
            setOutput(output);
            setIsLoading(false);
          });
        })
        .catch((error) => {
          setOutput(`Error: ${error.message}`);
          setIsLoading(false);
        });
    });
  }, [setOutput, setIsLoading]);

  return null;
};

export default PythonRunner;
