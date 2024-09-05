declare module "*.py" {
  const content: string;
  export default content;
}

interface Window {
  pyodide: {
    loadPackage: (packages: string[]) => Promise<void>;
    runPython: (code: string) => any;
  };
  languagePluginLoader: Promise<void>;
}
