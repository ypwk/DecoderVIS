import { useEffect, useState, useRef } from "react";
import * as THREE from "three"; // Import Three.js
import script from "./python/test.py";
import logo from "./logo.svg";
import "./App.css";

const App = () => {
  const [output, setOutput] = useState("(loading...)");
  const [isLoading, setIsLoading] = useState(true);
  const sceneRef = useRef<HTMLDivElement>(null); // Ref for attaching Three.js renderer

  // Function to run Python code in Pyodide
  const runScript = (code: string) => {
    window.pyodide.loadPackage([]).then(() => {
      const output = window.pyodide.runPython(code);
      setOutput(output);
      setIsLoading(false); // Hide loading once the script runs
    });
  };

  // Load Pyodide and then run the script
  useEffect(() => {
    window.languagePluginLoader.then(() => {
      fetch(script)
        .then((src) => src.text())
        .then(runScript)
        .catch((error) => {
          setOutput(`Error: ${error.message}`);
          setIsLoading(false); // Hide loading on error
        });
    });

    // Initialize Three.js scene
    if (sceneRef.current) {
      const scene = new THREE.Scene();
      const camera = new THREE.PerspectiveCamera(
        75,
        window.innerWidth / window.innerHeight,
        0.1,
        1000
      );
      const renderer = new THREE.WebGLRenderer();
      renderer.setSize(window.innerWidth, window.innerHeight);
      sceneRef.current.appendChild(renderer.domElement);

      const geometry = new THREE.BoxGeometry();
      const material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
      const cube = new THREE.Mesh(geometry, material);
      scene.add(cube);

      camera.position.z = 5;

      const animate = () => {
        requestAnimationFrame(animate);
        cube.rotation.x += 0.01;
        cube.rotation.y += 0.01;
        renderer.render(scene, camera);
      };

      animate();
    }
  }, []);

  return (
    <div className="App">
      <header className="App-header">
        {isLoading ? (
          <p>Loading Pyodide...</p> // Loading message or animation
        ) : (
          <p>
            5 + 7 = {output} {/* Display output once loaded */}
          </p>
        )}
        {/* Add a div to contain the Three.js scene */}
        <div ref={sceneRef}></div>
      </header>
    </div>
  );
};

export default App;
