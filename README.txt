Errores solucionados:  
1. Para el ortoedro no habeis reusado la geometria del cubo. Un ortoedro es un cubo escalado por lo que podríais usar el mismo VAO que el cubo. (✔)
2. No habeis hecho una piramide. La piramide tiene la base cuadrada, habeis hecho un tetraedro. (✔)
3. En las transformaciones no habeis aplicado el el delta time lo que provoca que todo el programa pueda funcionar distinto según el dispositivo. (✔)
4. Teneis un geometry shader que no hace nada. Podriais eliminarlo para ahorrar recursos y el resultado sería el mismo. (✔) 
5. El tiempo sigue pasando para el cambio de color de la piramide aunque estemos pausados, al despausar la piramide no mantiene el color que llevaba. (✔)
7. Los nombres del proyecto (MyFirstOpenGL) o los nombres de los shaders (MyFirstGeometryShader, MyFirstVertexShader...) no son muy claros. (✔)
8. Si quitamos los comentarios de código que teneis de los proyectos de clase hay mucha escasez de ellos. (✔)