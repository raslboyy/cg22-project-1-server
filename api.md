- lab-1 -- /upload:
    - request:
    - response:
        - 200 if OK and form-data body with image
        - else 400
    - example
- lab-2 -- /convert:
    - request:
        - image in form-data
        - from, to, channel in arg
    - response:
        - 200 if OK and form-data body with image
        - else 400
    - example
        - http://localhost:8080/convert?from=RGB&to=HSL&channel=first
- lab-3 -- /:
    - request:
        - image in form-data
        - from, to, channel in arg
    - response:
        - 200 if OK and form-data body with image
        - else 400
    - example
        - http://localhost:8080/convert?from=RGB&to=HSL&channel=first
- lab-4 -- /draw-line:
    - only in rgb(P6)
    - request:
        - image in form-data
        - x1, y1, x2, y2, thickness, alpha, r, g, b in arg (alpha -- 0..1, rgb -- 0..255)
    - response:
        - 200 if OK and form-data body with image
        - else 400
    - example
        - http://localhost:8080/convert?x1=1&y1=1...
- lab-5 -- /dithering:
    - add generate gradient to front(input -- w, h)
    - only in grayscale(P5)
    - request:
        - image in form-data
        - k, alg in arg (k -- 1..8, alg -- [atkinson, floyd_steinberg, ordered, random])
    - response:
        - 200 if OK and form-data body with image
        - else 400
    - example
        - http://localhost:8080/convert?k=5&alg=atkinson
- lab-6 -- /scaling:
    - only rgb(P6)
    - default h_new = 0
    - request:
        - image in form-data
        - x_shift, y_shift, alg, B, C, w_new, h_new in arg(x, y -- ?, alg
          -- [bc_splines, bi_linear, lanczos, nearest_neighbor], B, C -- ?)
    - response:
        - 200 if OK and form-data body with image
        - else 400
    - example
        - http://localhost:8080/convert?x_shift=...