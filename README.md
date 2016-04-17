# BES_Uebung02

## Fragen:

### 1.) Komplizierte IF-Anweisungen.
Beispiel:
```C
if (command == NULL || type == NULL || strlen(type) != 1 || (!(type[0] == 'w' || type[0] == 'r')))
{
	errno = EINVAL;
	return NULL;
}
```

### 2.) Was macht man im Fehlerfall falls nach einem fehlerhaften Aufruf noch ein Fehler eintritt?
Beispiel:
```C
if (pid == EXIT_ERROR)
{
	// errno gets set by fork
	
	// reset pid to avoid problems in mypclose()
	pid = NULL;

	if(close(fd[PIPE_WRITE]) == EXIT_ERROR)
	{
		// @todo now what?
	}
	if(close(fd[PIPE_READ]) == EXIT_ERROR)
	{
		// @todo now what?
	}
	return NULL;
}
```

### 3.) Wann schließt man welchen FILE * ?
Beispiel:
```C
if (_fp == NULL)
{
	// @todo do I have to close FILE * stream?
	errno = ECHILD;
	return EXIT_ERROR;
}

if (stream == NULL)
{
	// @todo do I have to close _fp?
	errno = EINVAL;
	return EXIT_ERROR;
}

if (stream != _fp)
{
	// @todo do I have to close stream and _fp?
	errno = EINVAL;
	return EXIT_ERROR;
}
```

### 4.) Muss ich nachdem ich _fp schließe auch noch den File Descriptor schließen?