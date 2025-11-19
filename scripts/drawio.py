from dataclasses import dataclass, field

@dataclass
class Pont:
    ptx: int
    pty: int
    dx: float = 0.0
    dy: float = 0.0
    z: int = 0

    def __repr__(self) -> str:
        return f"[{self.dx},{self.dy},{self.z},{self.ptx},{self.pty}]"


@dataclass
class Header:
    start: tuple[int, int]
    end: tuple[int, int]
    amount: int
    points: list[Pont] = field(default_factory=list)


def generate_points(int, header: Header) -> list[tuple[int, int]]:
    x0 = header.start[0]
    y0 = header.start[1]
    x1 = header.end[0]
    y1 = header.end[1]

    dx = 1.0 * (x1 - x0) / (header.amount - 1) if header.amount > 1 else 0
    dy = 1.0 * (y1 - y0) / (header.amount - 1) if header.amount > 1 else 0

    for i in range(header.amount):
        px = int(x0 + i * dx)
        py = int(y0 + i * dy)
        header.points.append(Pont(ptx=px, pty=py))

def print_points(headers: list[Header]):
    points_str = []
    for header in headers:
        for point in header.points:
            points_str.append(repr(point))
    res = ",".join(points_str)
    print(f'points=[{res}]')


def generate_arduino_pinout(headers: list[Header]):
    for header in headers:
        generate_points(int, header)

if __name__ == "__main__":
    headers = [
        # left side
        Header(start=(377, 314), end=(377, 472), amount=8),
        
        Header(start=(234, 514), end=(234, 583), amount=4),
        Header(start=(95, 604), end=(95, 672), amount=4),

        Header(start=(165, 712), end=(165, 870), amount=8),

        # right side
        Header(start=(1302, 234), end=(1302, 259), amount=2),
        Header(start=(1090, 280), end=(1090, 302), amount=2),
        Header(start=(1608, 323), end=(1608, 436), amount=6),

        Header(start=(1608, 467), end=(1608, 624), amount=8),

        Header(start=(1466, 663), end=(1466, 687), amount=2),
        Header(start=(1231, 708), end=(1231, 732), amount=2),
        Header(start=(1302, 754), end=(1302, 823), amount=4),
    ]
    generate_arduino_pinout(headers)
    print_points(headers)