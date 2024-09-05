interface OutputDisplayProps {
  output: string;
}

const OutputDisplay: React.FC<OutputDisplayProps> = ({ output }) => {
  return <p>{output}</p>;
};

export default OutputDisplay;
