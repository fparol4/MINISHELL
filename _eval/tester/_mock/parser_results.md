# Raw Parser Results

These mocks show the final parser shape before runner execution.
The parser only emits `CMD` and `PIPE` nodes. Redirections belong to `CMD`.

## echo hello

```c
{
	.type = CMD,
	.args = {"echo", "hello", NULL},
	.redir = NULL
}
```

## cat < input.txt

```c
{
	.type = CMD,
	.args = {"cat", NULL},
	.redir = {
		.type = IN_F,
		.target = "input.txt",
		.expand = true,
		.next = NULL
	}
}
```

## echo hello > out.txt

```c
{
	.type = CMD,
	.args = {"echo", "hello", NULL},
	.redir = {
		.type = OUT_T,
		.target = "out.txt",
		.expand = true,
		.next = NULL
	}
}
```

## echo hello >> out.txt

```c
{
	.type = CMD,
	.args = {"echo", "hello", NULL},
	.redir = {
		.type = OUT_A,
		.target = "out.txt",
		.expand = true,
		.next = NULL
	}
}
```

## cat << EOF

```c
{
	.type = CMD,
	.args = {"cat", NULL},
	.redir = {
		.type = IN_H,
		.target = "EOF",
		.expand = true,
		.next = NULL
	}
}
```

## cat << 'EOF'

```c
{
	.type = CMD,
	.args = {"cat", NULL},
	.redir = {
		.type = IN_H,
		.target = "EOF",
		.expand = false,
		.next = NULL
	}
}
```

## cat < in.txt > out.txt

```c
{
	.type = CMD,
	.args = {"cat", NULL},
	.redir = {
		.type = IN_F,
		.target = "in.txt",
		.expand = true,
		.next = {
			.type = OUT_T,
			.target = "out.txt",
			.expand = true,
			.next = NULL
		}
	}
}
```

## echo hello | wc -c

```c
{
	.type = PIPE,
	.left = {
		.type = CMD,
		.args = {"echo", "hello", NULL},
		.redir = NULL
	},
	.right = {
		.type = CMD,
		.args = {"wc", "-c", NULL},
		.redir = NULL
	}
}
```

## cat < in.txt | grep hi > out.txt

```c
{
	.type = PIPE,
	.left = {
		.type = CMD,
		.args = {"cat", NULL},
		.redir = {
			.type = IN_F,
			.target = "in.txt",
			.expand = true,
			.next = NULL
		}
	},
	.right = {
		.type = CMD,
		.args = {"grep", "hi", NULL},
		.redir = {
			.type = OUT_T,
			.target = "out.txt",
			.expand = true,
			.next = NULL
		}
	}
}
```

## echo first > a > b

```c
{
	.type = CMD,
	.args = {"echo", "first", NULL},
	.redir = {
		.type = OUT_T,
		.target = "a",
		.expand = true,
		.next = {
			.type = OUT_T,
			.target = "b",
			.expand = true,
			.next = NULL
		}
	}
}
```
